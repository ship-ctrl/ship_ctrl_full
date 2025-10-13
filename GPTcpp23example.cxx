#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <vector>
#include <unordered_map>
#include <format>
#include <expected>
#include <source_location>
#include <chrono>

// Forward declarations
class Ship;
class Protocol;

// ============================================================================
// Error Handling Types
// ============================================================================
enum class ErrorCode {
    Success = 0,
    InvalidMessage,
    OutOfRange,
    SystemFailure,
    CommunicationError
};

struct ErrorInfo {
    ErrorCode code;
    std::string message;
    std::source_location location;
    
    ErrorInfo(ErrorCode c, std::string msg, 
              std::source_location loc = std::source_location::current())
        : code(c), message(std::move(msg)), location(loc) {}
};

template<typename T>
using Result = std::expected<T, ErrorInfo>;

// ============================================================================
// Message Base and Concrete Types
// ============================================================================
class Message {
public:
    virtual ~Message() = default;
    virtual Result<void> validate() const = 0;
    virtual std::string name() const = 0;
    
protected:
    Message() = default;
};

class NavigationMessage : public Message {
private:
    double latitude_;
    double longitude_;
    double speed_;
    double heading_;
    
public:
    NavigationMessage(double lat, double lon, double spd, double hdg)
        : latitude_(lat), longitude_(lon), speed_(spd), heading_(hdg) {}
    
    Result<void> validate() const override {
        if (latitude_ < -90.0 || latitude_ > 90.0) {
            return std::unexpected(ErrorInfo{
                ErrorCode::OutOfRange, 
                std::format("Latitude {} out of range [-90, 90]", latitude_)
            });
        }
        if (longitude_ < -180.0 || longitude_ > 180.0) {
            return std::unexpected(ErrorInfo{
                ErrorCode::OutOfRange,
                std::format("Longitude {} out of range [-180, 180]", longitude_)
            });
        }
        if (speed_ < 0.0 || speed_ > 30.0) {
            return std::unexpected(ErrorInfo{
                ErrorCode::OutOfRange,
                std::format("Speed {} out of range [0, 30]", speed_)
            });
        }
        if (heading_ < 0.0 || heading_ >= 360.0) {
            return std::unexpected(ErrorInfo{
                ErrorCode::OutOfRange,
                std::format("Heading {} out of range [0, 360)", heading_)
            });
        }
        return {};
    }
    
    std::string name() const override { return "NavigationMessage"; }
    
    // Getters
    double latitude() const { return latitude_; }
    double longitude() const { return longitude_; }
    double speed() const { return speed_; }
    double heading() const { return heading_; }
};

class EngineMessage : public Message {
private:
    double main_engine_power_;
    bool emergency_stop_;
    bool backup_generator_;
    
public:
    EngineMessage(double power, bool emergency = false, bool backup = false)
        : main_engine_power_(power), emergency_stop_(emergency), backup_generator_(backup) {}
    
    Result<void> validate() const override {
        if (main_engine_power_ < 0.0 || main_engine_power_ > 100.0) {
            return std::unexpected(ErrorInfo{
                ErrorCode::OutOfRange,
                std::format("Engine power {} out of range [0, 100]", main_engine_power_)
            });
        }
        return {};
    }
    
    std::string name() const override { return "EngineMessage"; }
    
    // Getters
    double main_engine_power() const { return main_engine_power_; }
    bool emergency_stop() const { return emergency_stop_; }
    bool backup_generator() const { return backup_generator_; }
};

class SystemsMessage : public Message {
private:
    bool fire_system_active_;
    bool bilge_pump_active_;
    double fuel_level_;
    double water_level_;
    
public:
    SystemsMessage(bool fire_system, bool bilge_pump, double fuel, double water)
        : fire_system_active_(fire_system), bilge_pump_active_(bilge_pump),
          fuel_level_(fuel), water_level_(water) {}
    
    Result<void> validate() const override {
        if (fuel_level_ < 0.0 || fuel_level_ > 100.0) {
            return std::unexpected(ErrorInfo{
                ErrorCode::OutOfRange,
                std::format("Fuel level {} out of range [0, 100]", fuel_level_)
            });
        }
        if (water_level_ < 0.0 || water_level_ > 100.0) {
            return std::unexpected(ErrorInfo{
                ErrorCode::OutOfRange,
                std::format("Water level {} out of range [0, 100]", water_level_)
            });
        }
        return {};
    }
    
    std::string name() const override { return "SystemsMessage"; }
    
    // Getters
    bool fire_system_active() const { return fire_system_active_; }
    bool bilge_pump_active() const { return bilge_pump_active_; }
    double fuel_level() const { return fuel_level_; }
    double water_level() const { return water_level_; }
};

// ============================================================================
// Message Handler Interface and Implementations
// ============================================================================
class MessageHandler {
public:
    virtual ~MessageHandler() = default;
    virtual Result<void> handle(const Message& message, Ship& ship) const = 0;
    virtual bool canHandle(const Message& message) const = 0;
};

class NavigationHandler : public MessageHandler {
public:
    Result<void> handle(const Message& message, Ship& ship) const override;
    bool canHandle(const Message& message) const override {
        return dynamic_cast<const NavigationMessage*>(&message) != nullptr;
    }
};

class EngineHandler : public MessageHandler {
public:
    Result<void> handle(const Message& message, Ship& ship) const override;
    bool canHandle(const Message& message) const override {
        return dynamic_cast<const EngineMessage*>(&message) != nullptr;
    }
};

class SystemsHandler : public MessageHandler {
public:
    Result<void> handle(const Message& message, Ship& ship) const override;
    bool canHandle(const Message& message) const override {
        return dynamic_cast<const SystemsMessage*>(&message) != nullptr;
    }
};

// ============================================================================
// Ship Class - Represents Current State
// ============================================================================
class Ship {
private:
    // Navigation state
    double latitude_{0.0};
    double longitude_{0.0};
    double speed_{0.0};
    double heading_{0.0};
    
    // Engine state
    double main_engine_power_{0.0};
    bool emergency_stop_{false};
    bool backup_generator_{false};
    
    // Systems state
    bool fire_system_active_{false};
    bool bilge_pump_active_{false};
    double fuel_level_{100.0};
    double water_level_{0.0};
    
    // Statistics
    std::chrono::system_clock::time_point last_update_;
    size_t total_messages_processed_{0};
    
public:
    Ship() : last_update_(std::chrono::system_clock::now()) {}
    
    // Navigation methods
    void setNavigation(double lat, double lon, double spd, double hdg) {
        latitude_ = lat;
        longitude_ = lon;
        speed_ = spd;
        heading_ = hdg;
        updateTimestamp();
    }
    
    // Engine methods
    void setEngineState(double power, bool emergency, bool backup) {
        main_engine_power_ = power;
        emergency_stop_ = emergency;
        backup_generator_ = backup;
        updateTimestamp();
    }
    
    // Systems methods
    void setSystemsState(bool fire_system, bool bilge_pump, double fuel, double water) {
        fire_system_active_ = fire_system;
        bilge_pump_active_ = bilge_pump;
        fuel_level_ = fuel;
        water_level_ = water;
        updateTimestamp();
    }
    
    // Getters
    auto getNavigation() const { return std::tie(latitude_, longitude_, speed_, heading_); }
    auto getEngineState() const { return std::tie(main_engine_power_, emergency_stop_, backup_generator_); }
    auto getSystemsState() const { return std::tie(fire_system_active_, bilge_pump_active_, fuel_level_, water_level_); }
    auto lastUpdate() const { return last_update_; }
    size_t totalMessagesProcessed() const { return total_messages_processed_; }
    
    void incrementMessageCount() { ++total_messages_processed_; }
    
private:
    void updateTimestamp() {
        last_update_ = std::chrono::system_clock::now();
    }
};

// ============================================================================
// Handler Implementations
// ============================================================================
Result<void> NavigationHandler::handle(const Message& message, Ship& ship) const {
    if (auto* nav_msg = dynamic_cast<const NavigationMessage*>(&message)) {
        if (auto validation = nav_msg->validate(); !validation) {
            return validation;
        }
        
        ship.setNavigation(
            nav_msg->latitude(),
            nav_msg->longitude(),
            nav_msg->speed(),
            nav_msg->heading()
        );
        ship.incrementMessageCount();
        
        std::cout << std::format("[Navigation] Position: ({:.4f}, {:.4f}), Speed: {:.1f}, Heading: {:.1f}\n",
            nav_msg->latitude(), nav_msg->longitude(), nav_msg->speed(), nav_msg->heading());
        return {};
    }
    return std::unexpected(ErrorInfo{
        ErrorCode::InvalidMessage,
        "NavigationHandler received incompatible message type"
    });
}

Result<void> EngineHandler::handle(const Message& message, Ship& ship) const {
    if (auto* engine_msg = dynamic_cast<const EngineMessage*>(&message)) {
        if (auto validation = engine_msg->validate(); !validation) {
            return validation;
        }
        
        ship.setEngineState(
            engine_msg->main_engine_power(),
            engine_msg->emergency_stop(),
            engine_msg->backup_generator()
        );
        ship.incrementMessageCount();
        
        std::cout << std::format("[Engine] Power: {:.1f}%, Emergency: {}, Backup: {}\n",
            engine_msg->main_engine_power(),
            engine_msg->emergency_stop() ? "ON" : "OFF",
            engine_msg->backup_generator() ? "ON" : "OFF");
        return {};
    }
    return std::unexpected(ErrorInfo{
        ErrorCode::InvalidMessage,
        "EngineHandler received incompatible message type"
    });
}

Result<void> SystemsHandler::handle(const Message& message, Ship& ship) const {
    if (auto* systems_msg = dynamic_cast<const SystemsMessage*>(&message)) {
        if (auto validation = systems_msg->validate(); !validation) {
            return validation;
        }
        
        ship.setSystemsState(
            systems_msg->fire_system_active(),
            systems_msg->bilge_pump_active(),
            systems_msg->fuel_level(),
            systems_msg->water_level()
        );
        ship.incrementMessageCount();
        
        std::cout << std::format("[Systems] Fuel: {:.1f}%, Water: {:.1f}%, Fire System: {}, Bilge Pump: {}\n",
            systems_msg->fuel_level(),
            systems_msg->water_level(),
            systems_msg->fire_system_active() ? "ACTIVE" : "INACTIVE",
            systems_msg->bilge_pump_active() ? "ON" : "OFF");
        return {};
    }
    return std::unexpected(ErrorInfo{
        ErrorCode::InvalidMessage,
        "SystemsHandler received incompatible message type"
    });
}

// ============================================================================
// Protocol Class - Aggregates Message Handlers
// ============================================================================
class Protocol {
private:
    std::vector<std::unique_ptr<MessageHandler>> handlers_;
    Ship& ship_;
    
public:
    explicit Protocol(Ship& ship) : ship_(ship) {
        // Register default handlers
        registerHandler(std::make_unique<NavigationHandler>());
        registerHandler(std::make_unique<EngineHandler>());
        registerHandler(std::make_unique<SystemsHandler>());
    }
    
    void registerHandler(std::unique_ptr<MessageHandler> handler) {
        handlers_.push_back(std::move(handler));
    }
    
    Result<void> processMessage(const Message& message) {
        std::cout << std::format("Processing {}...\n", message.name());
        
        // Validate message first
        if (auto validation = message.validate(); !validation) {
            return validation;
        }
        
        // Find appropriate handler
        for (const auto& handler : handlers_) {
            if (handler->canHandle(message)) {
                return handler->handle(message, ship_);
            }
        }
        
        return std::unexpected(ErrorInfo{
            ErrorCode::InvalidMessage,
            std::format("No handler found for message type: {}", message.name())
        });
    }
    
    template<typename T, typename... Args>
    Result<void> createAndProcess(Args&&... args) {
        T message(std::forward<Args>(args)...);
        return processMessage(message);
    }
};

// ============================================================================
// Utility Functions
// ============================================================================
void printShipState(const Ship& ship) {
    auto [lat, lon, spd, hdg] = ship.getNavigation();
    auto [power, emergency, backup] = ship.getEngineState();
    auto [fire, bilge, fuel, water] = ship.getSystemsState();
    
    std::cout << "\n=== SHIP CURRENT STATE ===\n";
    std::cout << std::format("Navigation: Lat={:.4f}, Lon={:.4f}, Speed={:.1f}, Heading={:.1f}\n", 
        lat, lon, spd, hdg);
    std::cout << std::format("Engine: Power={:.1f}%, Emergency={}, Backup={}\n",
        power, emergency ? "ON" : "OFF", backup ? "ON" : "OFF");
    std::cout << std::format("Systems: Fuel={:.1f}%, Water={:.1f}%, Fire System={}, Bilge Pump={}\n",
        fuel, water, fire ? "ACTIVE" : "INACTIVE", bilge ? "ON" : "OFF");
    std::cout << std::format("Statistics: Messages processed={}\n", 
        ship.totalMessagesProcessed());
    std::cout << "==========================\n\n";
}

void handleResult(const Result<void>& result) {
    if (!result) {
        const auto& error = result.error();
        std::cout << std::format("ERROR [{}]: {} at {}:{}\n",
            static_cast<int>(error.code),
            error.message,
            error.location.file_name(),
            error.location.line());
    }
}

// ============================================================================
// Main Application
// ============================================================================
int main() {
    try {
        // Create ship and protocol
        Ship ship;
        Protocol protocol(ship);
        
        std::cout << "=== Ship Control System Started ===\n";
        printShipState(ship);
        
        // Process various messages
        std::cout << "=== Processing Control Messages ===\n";
        
        // Navigation messages
        handleResult(protocol.createAndProcess<NavigationMessage>(40.7128, -74.0060, 15.5, 45.0));
        handleResult(protocol.createAndProcess<NavigationMessage>(41.8781, -87.6298, 20.0, 90.0));
        
        // Engine messages
        handleResult(protocol.createAndProcess<EngineMessage>(75.0, false, true));
        handleResult(protocol.createAndProcess<EngineMessage>(0.0, true, false)); // Emergency stop
        
        // Systems messages
        handleResult(protocol.createAndProcess<SystemsMessage>(false, true, 85.5, 2.5));
        handleResult(protocol.createAndProcess<SystemsMessage>(true, false, 45.0, 15.0));
        
        // Test error cases
        std::cout << "\n=== Testing Error Cases ===\n";
        handleResult(protocol.createAndProcess<NavigationMessage>(100.0, -74.0060, 15.5, 45.0)); // Invalid latitude
        handleResult(protocol.createAndProcess<EngineMessage>(150.0, false, true)); // Invalid power level
        
        // Final state
        printShipState(ship);
        std::cout << "=== Ship Control System Finished ===\n";
        
    } catch (const std::exception& e) {
        std::cout << std::format("Unexpected error: {}\n", e.what());
        return 1;
    }
    
    return 0;
}